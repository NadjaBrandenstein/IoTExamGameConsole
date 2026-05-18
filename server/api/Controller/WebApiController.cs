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

    [HttpGet("whackabird")]
    public async Task<RealtimeListenResponse<List<Whackabirdscore>>> GetWhackabirdScores([FromQuery] string connectionId)
    {
        var group = "WhackamoleScores";

        await backplane.Groups.AddToGroupAsync(connectionId, group);

        realtimeManager.Subscribe<MyDbContext>(connectionId, group,
            criteria: snapshot => snapshot.HasChanges<Whackabirdscore>(),
            query: context =>
            {
                var result = context.Whackabirdscores
                    .GroupBy(x => x.PlayerName)
                    .Select(g => g.OrderByDescending(x => x.Score).First())
                    .OrderByDescending(x => x.Score)
                    .Take(10)
                    .ToList();

                return Task.FromResult<object?>(result);
            });

        var initial = await ctx.Whackabirdscores
            .GroupBy(x => x.PlayerName)
            .Select(g => g.OrderByDescending(x => x.Score).First())
            .OrderByDescending(x => x.Score)
            .Take(10)
            .ToListAsync();

        return new RealtimeListenResponse<List<Whackabirdscore>>(group, initial);
    }

    
    [HttpGet("redbirdgreenbird")]
    public async Task<RealtimeListenResponse<List<Redbirdgreenbirdscore>>> GetRedbirdScores([FromQuery] string connectionId)
    {
        var group = "RedBirdGreenBirdScores";

        await backplane.Groups.AddToGroupAsync(connectionId, group);

        realtimeManager.Subscribe<MyDbContext>(connectionId, group,
            criteria: snapshot => snapshot.HasChanges<Redbirdgreenbirdscore>(),
            query: context =>
            {
                var result = context.Redbirdgreenbirdscores
                    .GroupBy(x => x.PlayerName)
                    .Select(g => g.OrderByDescending(x => x.Score).First())
                    .OrderByDescending(x => x.Score)
                    .Take(10)
                    .ToList();

                return Task.FromResult<object?>(result);
            });

        var initial = await ctx.Redbirdgreenbirdscores
            .GroupBy(x => x.PlayerName)
            .Select(g => g.OrderByDescending(x => x.Score).First())
            .OrderByDescending(x => x.Score)
            .Take(10)
            .ToListAsync();

        return new RealtimeListenResponse<List<Redbirdgreenbirdscore>>(group, initial);
    }

    
    [HttpGet("birdiesays")]
    public async Task<RealtimeListenResponse<List<Birdiesaysscore>>> GetBirdieSaysScores([FromQuery] string connectionId)
    {
        var group = "BirdieSaysScores";

        await backplane.Groups.AddToGroupAsync(connectionId, group);

        realtimeManager.Subscribe<MyDbContext>(connectionId, group,
            criteria: snapshot => snapshot.HasChanges<Birdiesaysscore>(),
            query: async context =>
            {
                return context.Birdiesaysscores
                    .OrderByDescending(x => x.Score)
                    .Take(10)
                    .ToList();
                
                //return Task.FromResult<object?>(result);
                //return (object?)result;
            });

        var initial = ctx.Birdiesaysscores
            .OrderByDescending(x => x.Score)
            .Take(10)
            .ToList();

        return new RealtimeListenResponse<List<Birdiesaysscore>>(group, initial);
    }
}