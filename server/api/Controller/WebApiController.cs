using dataaccess.Entity;
using dataaccess.MyDbContext;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using StateleSSE.AspNetCore;
using StateleSSE.AspNetCore.EfRealtime;
using StateleSSE.AspNetCore.GroupRealtime;

namespace api.Controller;

[ApiController]
[Route("api/[controller]")]
public class WebApiController(
    ISseBackplane backplane,
    IRealtimeManager realtimeManager,
    MyDbContext ctx) : RealtimeControllerBase(backplane)
{

    [HttpGet("whackamole")]
    public async Task<RealtimeListenResponse<List<Whackamolescore>>> GetWhackamoleScores(string connectionId)
    {
        var group = "WhackamoleScores";

        await backplane.Groups.AddToGroupAsync(connectionId, group);

        realtimeManager.Subscribe<MyDbContext>(connectionId, group,
            criteria: snapshot => snapshot.HasChanges<Whackamolescore>(),
            query: context =>
            {
                var result = context.Whackamolescores
                    .GroupBy(x => x.PlayerName)
                    .Select(g => g.OrderByDescending(x => x.Score).First())
                    .OrderByDescending(x => x.Score)
                    .Take(10)
                    .ToList();

                return Task.FromResult<object?>(result);
            });

        var initial = await ctx.Whackamolescores
            .GroupBy(x => x.PlayerName)
            .Select(g => g.OrderByDescending(x => x.Score).First())
            .OrderByDescending(x => x.Score)
            .Take(10)
            .ToListAsync();

        return new RealtimeListenResponse<List<Whackamolescore>>(group, initial);
    }

    
    [HttpGet("redlightgreenlight")]
    public async Task<RealtimeListenResponse<List<Redlightgreenlightscore>>> GetRedLightScores(string connectionId)
    {
        var group = "RedLightGreenLightScores";

        await backplane.Groups.AddToGroupAsync(connectionId, group);

        realtimeManager.Subscribe<MyDbContext>(connectionId, group,
            criteria: snapshot => snapshot.HasChanges<Redlightgreenlightscore>(),
            query: context =>
            {
                var result = context.Redlightgreenlightscores
                    .GroupBy(x => x.PlayerName)
                    .Select(g => g.OrderByDescending(x => x.Score).First())
                    .OrderByDescending(x => x.Score)
                    .Take(10)
                    .ToList();

                return Task.FromResult<object?>(result);
            });

        var initial = await ctx.Redlightgreenlightscores
            .GroupBy(x => x.PlayerName)
            .Select(g => g.OrderByDescending(x => x.Score).First())
            .OrderByDescending(x => x.Score)
            .Take(10)
            .ToListAsync();

        return new RealtimeListenResponse<List<Redlightgreenlightscore>>(group, initial);
    }

    
    [HttpGet("simonsays")]
    public async Task<RealtimeListenResponse<List<Simonsaysscore>>> GetSimonSaysScores(string connectionId)
    {
        var group = "SimonSaysScores";

        await backplane.Groups.AddToGroupAsync(connectionId, group);

        realtimeManager.Subscribe<MyDbContext>(connectionId, group,
            criteria: snapshot => snapshot.HasChanges<Simonsaysscore>(),
            query: context =>
            {
                var result = context.Simonsaysscores
                    .GroupBy(x => x.PlayerName)
                    .Select(g => g.OrderByDescending(x => x.Score).First())
                    .OrderByDescending(x => x.Score)
                    .Take(10)
                    .ToList();

                return Task.FromResult<object?>(result);
            });

        var initial = await ctx.Simonsaysscores
            .GroupBy(x => x.PlayerName)
            .Select(g => g.OrderByDescending(x => x.Score).First())
            .OrderByDescending(x => x.Score)
            .Take(10)
            .ToListAsync();

        return new RealtimeListenResponse<List<Simonsaysscore>>(group, initial);
    }
}