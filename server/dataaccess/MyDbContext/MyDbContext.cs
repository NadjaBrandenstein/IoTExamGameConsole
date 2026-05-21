using Microsoft.EntityFrameworkCore;
using dataaccess.Entity;

namespace dataaccess.MyDbContext;

public partial class MyDbContext : DbContext
{
    public MyDbContext(DbContextOptions<MyDbContext> options)
        : base(options)
    {
    }

    public virtual DbSet<Birdiesaysscore> Birdiesaysscores { get; set; }

    public virtual DbSet<Redbirdgreenbirdscore> Redbirdgreenbirdscores { get; set; }

    public virtual DbSet<Whackabirdscore> Whackabirdscores { get; set; }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<Birdiesaysscore>(entity =>
        {
            entity.HasKey(e => e.Id).HasName("birdiesaysscores_pkey");

            entity.ToTable("birdiesaysscores", "gameconsole");

            entity.HasIndex(e => e.Score, "idx_simon_score").IsDescending();

            entity.Property(e => e.Id)
                .HasDefaultValueSql("nextval('birdiesaysscores_id_seq'::regclass)")
                .HasColumnName("id");
            entity.Property(e => e.CreatedAt)
                .HasDefaultValueSql("CURRENT_TIMESTAMP")
                .HasColumnName("created_at");
            entity.Property(e => e.PlayerName).HasColumnName("player_name");
            entity.Property(e => e.Score).HasColumnName("score");
        });

        modelBuilder.Entity<Redbirdgreenbirdscore>(entity =>
        {
            entity.HasKey(e => e.Id).HasName("redbirdgreenbirdscores_pkey");

            entity.ToTable("redbirdgreenbirdscores", "gameconsole");

            entity.HasIndex(e => e.Score, "idx_redlight_score").IsDescending();

            entity.Property(e => e.Id)
                .HasDefaultValueSql("nextval('redbirdgreenbirdscores_id_seq'::regclass)")
                .HasColumnName("id");
            entity.Property(e => e.CreatedAt)
                .HasDefaultValueSql("CURRENT_TIMESTAMP")
                .HasColumnName("created_at");
            entity.Property(e => e.PlayerName).HasColumnName("player_name");
            entity.Property(e => e.Score).HasColumnName("score");
        });

        modelBuilder.Entity<Whackabirdscore>(entity =>
        {
            entity.HasKey(e => e.Id).HasName("whackabirdscores_pkey");

            entity.ToTable("whackabirdscores", "gameconsole");

            entity.HasIndex(e => e.Score, "idx_whack_score").IsDescending();

            entity.Property(e => e.Id)
                .HasDefaultValueSql("nextval('whackabirdscores_id_seq'::regclass)")
                .HasColumnName("id");
            entity.Property(e => e.CreatedAt)
                .HasDefaultValueSql("CURRENT_TIMESTAMP")
                .HasColumnName("created_at");
            entity.Property(e => e.PlayerName).HasColumnName("player_name");
            entity.Property(e => e.Score).HasColumnName("score");
        });

        OnModelCreatingPartial(modelBuilder);
    }

    partial void OnModelCreatingPartial(ModelBuilder modelBuilder);
}
