using System;
using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;
using dataaccess.Entity;

namespace dataaccess.MyDbContext;

public partial class MyDbContext : DbContext
{
    public MyDbContext(DbContextOptions<MyDbContext> options)
        : base(options)
    {
    }

    public virtual DbSet<Redlightgreenlightscore> Redlightgreenlightscores { get; set; }

    public virtual DbSet<Simonsaysscore> Simonsaysscores { get; set; }

    public virtual DbSet<Whackamolescore> Whackamolescores { get; set; }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<Redlightgreenlightscore>(entity =>
        {
            entity.HasKey(e => e.Id).HasName("redlightgreenlightscores_pkey");

            entity.ToTable("redlightgreenlightscores", "gameconsole");

            entity.HasIndex(e => e.Score, "idx_redlight_score").IsDescending();

            entity.Property(e => e.Id)
                .HasDefaultValueSql("nextval('redlightgreenlightscores_id_seq'::regclass)")
                .HasColumnName("id");
            entity.Property(e => e.CreatedAt)
                .HasDefaultValueSql("CURRENT_TIMESTAMP")
                .HasColumnName("created_at");
            entity.Property(e => e.PlayerName).HasColumnName("player_name");
            entity.Property(e => e.Score).HasColumnName("score");
        });

        modelBuilder.Entity<Simonsaysscore>(entity =>
        {
            entity.HasKey(e => e.Id).HasName("simonsaysscores_pkey");

            entity.ToTable("simonsaysscores", "gameconsole");

            entity.HasIndex(e => e.Score, "idx_simon_score").IsDescending();

            entity.Property(e => e.Id)
                .HasDefaultValueSql("nextval('simonsaysscores_id_seq'::regclass)")
                .HasColumnName("id");
            entity.Property(e => e.CreatedAt)
                .HasDefaultValueSql("CURRENT_TIMESTAMP")
                .HasColumnName("created_at");
            entity.Property(e => e.PlayerName).HasColumnName("player_name");
            entity.Property(e => e.Score).HasColumnName("score");
        });

        modelBuilder.Entity<Whackamolescore>(entity =>
        {
            entity.HasKey(e => e.Id).HasName("whackamolescores_pkey");

            entity.ToTable("whackamolescores", "gameconsole");

            entity.HasIndex(e => e.Score, "idx_whack_score").IsDescending();

            entity.Property(e => e.Id)
                .HasDefaultValueSql("nextval('whackamolescores_id_seq'::regclass)")
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
