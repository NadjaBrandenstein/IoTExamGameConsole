#!/bin/bash
set -a
source .env
set +a

dotnet tool install -g dotnet-ef
dotnet ef dbcontext scaffold "Host=ep-curly-lake-alpch7en-pooler.c-3.eu-central-1.aws.neon.tech; Database=neondb; Username=neondb_owner; Password=npg_JmcEYfD02Pdj; SSL Mode=VerifyFull; Channel Binding=Require;" Npgsql.EntityFrameworkCore.PostgreSQL --context MyDbContext --no-onconfiguring --context-dir MyDbContext --output-dir Entity --schema gameconsole --force