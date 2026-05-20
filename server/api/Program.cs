using System.Text.Json;
using System.Text.Json.Serialization;
using api;
using api.Service;
// using api.Security;
// using api.Service;
// using dataaccess.Entity;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using dataaccess.MyDbContext;
// using dataaccess.Repositories;
using Mqtt.Controllers;
using MQTTnet;
using StateleSSE.AspNetCore;
using StateleSSE.AspNetCore.GroupRealtime;

public class Program
{
    public static void ConfigureServices(IServiceCollection services, IConfiguration configuration, WebApplicationBuilder builder)
    {
        var appOptions = services.AddAppOptions(configuration);

        // Use concrete AppDbContext instead of abstract DbContext
        var connectionString = appOptions.DbConnectionString;
        
        // Database
        // builder.Services.AddDbContext<MyDbContext>(options =>
        // {
        //     options.UseNpgsql(
        //         builder.Configuration.GetConnectionString("DefaultConnection"));
        // });
        builder.Services.AddDbContext<MyDbContext>((sp, options) =>
        {
            options.AddEfRealtimeInterceptor(sp);
            options.UseNpgsql(connectionString)
            .UseQueryTrackingBehavior(QueryTrackingBehavior.NoTracking);
        });

        // Repositories
        //builder.Services.AddScoped<IRepository<Login>, LoginRepository>();
        //builder.Services.AddScoped<IRepository<User>, UserRepository>();

        // Services
        //builder.Services.AddScoped<IPasswordHasher<Login>, NSecArgon2IdPasswordHasher>();
        builder.Services.AddScoped<ICommandService, CommandService>();
        //builder.Services.AddScoped<IAuthService, AuthService>();
        //builder.Services.AddScoped<ITokenService, JwtService>();
        
        // MQTT service
        // builder.Services.AddScoped<IMqttClientService, MqttClientService>();
        builder.Services.AddMqttControllers();
        
        // Authentication & Authorization
        builder.Services.AddAuthentication(options =>
        {
            options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
            options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
            options.DefaultScheme = JwtBearerDefaults.AuthenticationScheme;
            options.DefaultSignInScheme = JwtBearerDefaults.AuthenticationScheme;
        });
        // .AddJwtBearer(options =>
        // {
        //     options.TokenValidationParameters = JwtService.ValidationParameters(builder.Configuration);
        //
        //     // Debug logging
        //     options.Events = new JwtBearerEvents
        //     {
        //         OnAuthenticationFailed = context =>
        //         {
        //             Console.WriteLine($"Authentication failed: {context.Exception}");
        //             return Task.CompletedTask;
        //         },
        //         OnTokenValidated = context =>
        //         {
        //             Console.WriteLine("Token Validated Successfully");
        //             return Task.CompletedTask;
        //         }
        //     };
    //});

        builder.Services.AddAuthorization();

        // SSE 
        builder.Services.AddInMemorySseBackplane();
        builder.Services.AddEfRealtime();
        builder.Services.AddGroupRealtime();
       
        // Controllers & OpenAPI / Swagger
        builder.Services.AddControllers().AddJsonOptions(options =>
        {
            options.JsonSerializerOptions.PropertyNamingPolicy = JsonNamingPolicy.CamelCase;
            options.JsonSerializerOptions.DictionaryKeyPolicy = JsonNamingPolicy.CamelCase;
            options.JsonSerializerOptions.ReferenceHandler = ReferenceHandler.IgnoreCycles;
            options.JsonSerializerOptions.PropertyNameCaseInsensitive = true;
        });
        
        // OpenAPI / Swagger
        builder.Services.AddEndpointsApiExplorer();
        builder.Services.AddOpenApiDocument(); // no DefaultPropertyNameHandling needed

        builder.Services.AddProblemDetails();
        
        // CORS
        builder.Services.AddCors(options =>
        {
            options.AddPolicy("FrontendPolicy", policy =>
            {
                policy
                    .WithOrigins(
                        "https://birdbox-games.fly.dev",
                        "http://localhost:5173",
                        "http://localhost:5174"
                    )
                    .AllowAnyHeader()
                    .AllowAnyMethod();
            });
        });

    }

    public static async Task Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);

        // Configure services
        ConfigureServices(builder.Services, builder.Configuration, builder);

        var app = builder.Build();

        // Middleware pipeline
        app.UseRouting();
        app.UseCors("FrontendPolicy");

        app.UseAuthentication();
        app.UseAuthorization();
        
        app.UseOpenApi();
        app.UseSwaggerUi();
        
        if (app.Environment.IsDevelopment())
        {
            await app.GenerateApiClientsFromOpenApi("/../../client/src/generated-ts-client.ts");
        }

        app.MapControllers();
        app.UseExceptionHandler();
        
        // MQTT connect
        var mqtt = app.Services.GetRequiredService<IMqttClientService>();
        var mqttConfig = builder.Configuration.GetSection("Mqtt");

        var host = mqttConfig["Host"] ?? "mqtt.flespi.io";
        var port = int.Parse(mqttConfig["Port"] ?? "1883");
        var token = mqttConfig["Token"];
        
        await mqtt.ConnectAsync(
            host: host,
            port: port,
            username: token,
            password: null,
            useTls: false
        );

        await mqtt.SubscribeAsync("iot/game/+/score");

        Console.WriteLine($"Connected to MQTT broker {host}:{port}");
        Console.WriteLine("Subscribed to score topics");

        await app.RunAsync();
    }
}