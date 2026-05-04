#!/bin/bash
set -a
source .env
set +a

dotnet tool install -g dotnet-ef
dotnet ef dbcontext scaffold "insert string here" Npgsql.EntityFrameworkCore.PostgreSQL --context MyDbContext --no-onconfiguring --context-dir MyDbContext --output-dir Entity --schema gameconsole --force