using System;
using System.Collections.Generic;

namespace dataaccess.Entity;

public partial class Simonsaysscore
{
    public int Id { get; set; }

    public string PlayerName { get; set; } = null!;

    public int Score { get; set; }

    public DateTime? CreatedAt { get; set; }
}
