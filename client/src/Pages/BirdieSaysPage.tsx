import "../CSS/BirdieSays.css";
import "../CSS/App.css";

import { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";

import { useCommand } from "../Hooks/useCommands.ts";
import { webClient } from "../api-clients.ts";

import type {Birdiesaysscore} from "../generated-ts-client";

export default function BirdieSaysPage() {

    const location = useLocation();

    const name = location.state?.name || "Player";

    const { sendCommand } = useCommand();

    const [scores, setScores] =
        useState<Birdiesaysscore[]>([]);

    const startGame = () => {

        console.log("Sending player:", name);

        if (!name) {
            return alert("Please enter a name!");
        }

        sendCommand("firebeetle01", {
            game: "birdiesays",
            action: "start",
            playerName: name

        });
    };

    useEffect(() => {

        const connectionId = crypto.randomUUID();

        // Subscribe backend
        const subscribe = async () => {

            try {

                const response =
                    await webClient.getBirdieSaysScores(connectionId);

                setScores(response.data || []);

            } catch (err) {

                console.error(
                    "Failed to subscribe",
                    err
                );
            }
        };

        subscribe();

        // Open SSE stream
        const eventSource = new EventSource(
            `http://localhost:5000/api/WebApi/sse?connectionId=${connectionId}`
        );

        eventSource.onmessage = (event) => {

            const payload =
                JSON.parse(event.data);

            console.log("SSE:", payload);

            if (
                payload.groupName ===
                "BirdieSaysScores"
            ) {

                setScores(payload.data);
            }
        };

        eventSource.onerror = (err) => {

            console.error("SSE error", err);
        };

        return () => {

            eventSource.close();
        };

    }, []);

    // useEffect(() => {
    //
    //     const loadScores = async () => {
    //
    //         try {
    //
    //             const response =
    //                 await webClient.getBirdieSaysScores(undefined);
    //
    //             setScores(response.data || []);
    //
    //         } catch (err) {
    //
    //             console.error("Failed to load scores", err);
    //         }
    //     };
    //
    //     loadScores();
    //
    // }, []);

    return (
        <div className="simon-page">

            <div className="simon-rules">
                Rules
            </div>

            <div className="simon-content">

                <h1>BIRDIE SAYS</h1>

                <div className="simon-score-box">

                    <table className="score-table">

                        <thead>
                        <tr>
                            <th>Name</th>
                            <th>Score</th>
                        </tr>
                        </thead>

                        <tbody>

                        {scores.map((player) => (

                            <tr key={player.id}>
                                <td>{player.playerName}</td>
                                <td>{player.score}</td>
                            </tr>

                        ))}

                        </tbody>

                    </table>

                </div>

                <button
                    className="simon-start-btn"
                    onClick={startGame}
                >
                    START GAME
                </button>

            </div>
        </div>
    );
}