using System.Text.Json;
using dataaccess.Entity;
using dataaccess.MyDbContext;
using Microsoft.EntityFrameworkCore;
using Mqtt.Controllers;

namespace api.Controller;

public class DeviceController(
    ILogger<DeviceController> logger,
    MyDbContext ctx) : MqttController
{
    
    [MqttRoute("farm/EB_Windmill/windmill/{deviceId}/score")]
    public async Task HandleScore(string deviceId, GameScoreDto dto)
    {
        logger.LogInformation($"Game: {dto.Game}, Player: {dto.PlayerName}, Score: {dto.Score}");

        var game = dto.Game.ToLower();

        switch (game)
        {
            case "whackamole":
                ctx.Whackamolescores.Add(new Whackamolescore
                {
                    PlayerName = dto.PlayerName,
                    Score = dto.Score,
                    CreatedAt = DateTime.UtcNow
                });
                break;

            case "redlightgreenlight":
                ctx.Redlightgreenlightscores.Add(new Redlightgreenlightscore
                {
                    PlayerName = dto.PlayerName,
                    Score = dto.Score,
                    CreatedAt = DateTime.UtcNow
                });
                break;

            case "simonsays":
                ctx.Simonsaysscores.Add(new Simonsaysscore
                {
                    PlayerName = dto.PlayerName,
                    Score = dto.Score,
                    CreatedAt = DateTime.UtcNow
                });
                break;

            default:
                logger.LogWarning($"Unknown game: {dto.Game}");
                return;
        }

        await ctx.SaveChangesAsync();
    }
}