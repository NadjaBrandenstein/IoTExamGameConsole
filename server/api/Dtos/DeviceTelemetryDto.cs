public class DeviceTelemetryDto
{
    public string DeviceId { get; set; } = string.Empty;

    // Example fields — adjust to what your C++ actually sends
    public string? Game { get; set; }
    public string? Status { get; set; }
    public int? Score { get; set; }
    public DateTime Timestamp { get; set; } = DateTime.UtcNow;
}