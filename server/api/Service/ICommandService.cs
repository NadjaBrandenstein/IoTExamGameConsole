public interface ICommandService
{
    Task SendCommandAsync(string turbineId, GameCommandDto command);
}