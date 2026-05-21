using Microsoft.AspNetCore.Mvc;

namespace api.Controller;

[ApiController]
[Route("api/[controller]")]
[Produces("application/json")]
public class CommandController(
    ICommandService service)
    : ControllerBase
{
    [HttpPost("{deviceId}")]
    public async Task<IActionResult> SendCommand(
        string deviceId,
        [FromBody] GameCommandDto command)
    {
        try
        {
            await service.SendCommandAsync(deviceId, command);
            return Ok(new { message = "Command sent successfully" });
        }
        catch (Exception ex)
        {
            return StatusCode(500, new
            {
                message = ex.Message,
                stackTrace = ex.StackTrace
            });
        }
    }
}