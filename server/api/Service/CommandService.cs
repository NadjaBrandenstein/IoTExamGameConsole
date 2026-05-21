using System.Text.Json;
using dataaccess.MyDbContext;
using Mqtt.Controllers;

namespace api.Service;

public class CommandService (
    IMqttClientService mqtt,
    MyDbContext ctx)
    : ICommandService
{
    public async Task SendCommandAsync(string deviceId, GameCommandDto command)
    {
        // Validate input
        ValidateCommand(command);

        Console.WriteLine($"Publishing command to device ID: {deviceId}");

        // Normalize values (important for C++ side)
        var normalizedCommand = new GameCommandDto
        {
            Game = command.Game.ToLower(),
            Action = command.Action.ToLower(),
            PlayerName = command.PlayerName
        };

        // Topic (update this if your device expects something else)
        var topic = $"iot/game/{deviceId}/command";

        // Serialize payload
        var payload = JsonSerializer.Serialize(normalizedCommand, new JsonSerializerOptions
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            DefaultIgnoreCondition = System.Text.Json.Serialization.JsonIgnoreCondition.WhenWritingNull
        });

        Console.WriteLine($"Payload: {payload}");

        // Publish to MQTT
        await mqtt.PublishAsync(topic, payload);
        
        await ctx.SaveChangesAsync();
    }

    private void ValidateCommand(GameCommandDto command)
    {
        if (string.IsNullOrWhiteSpace(command.Game))
            throw new Exception("Game is required");

        if (string.IsNullOrWhiteSpace(command.Action))
            throw new Exception("Action is required");

        var validGames = new[] 
        { 
            "birdiesays", 
            "whackabird", 
            "redbirdgreenbird" 
        };

        if (!validGames.Contains(command.Game.ToLower()))
            throw new Exception("Invalid game");

        var validActions = new[] { "start", "stop" };

        if (!validActions.Contains(command.Action.ToLower()))
            throw new Exception("Invalid action");
    }
}