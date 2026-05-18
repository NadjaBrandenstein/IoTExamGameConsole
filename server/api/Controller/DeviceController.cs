using System.Text.Json;
using dataaccess.Entity;
using dataaccess.MyDbContext;
using Microsoft.EntityFrameworkCore;
using Mqtt.Controllers;
using StateleSSE.AspNetCore;
using StateleSSE.AspNetCore.EfRealtime;

namespace api.Controller;

public class DeviceController(
    ILogger<DeviceController> logger,
    MyDbContext ctx) : MqttController
{
    
    [MqttRoute("iot/game/{deviceId}/score")]
    public async Task HandleScore(string deviceId, GameScoreDto dto)
    {
        logger.LogInformation($"Game: {dto.Game}, Player: {dto.PlayerName}, Score: {dto.Score}");

        var game = dto.Game.ToLower();

        switch (game)
        {
            case "whackabird":
                ctx.Whackabirdscores.Add(new Whackabirdscore
                {
                    PlayerName = dto.PlayerName,
                    Score = dto.Score,
                    CreatedAt = DateTime.UtcNow
                });
                break;

            case "redbirdgreenbird":
                ctx.Redbirdgreenbirdscores.Add(new Redbirdgreenbirdscore
                {
                    PlayerName = dto.PlayerName,
                    Score = dto.Score,
                    CreatedAt = DateTime.UtcNow
                });
                break;

            case "birdiesays":
                ctx.Birdiesaysscores.Add(new Birdiesaysscore
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
        
        Console.WriteLine("Score saved to database");
    }
}