import "../CSS/BirdieSays.css";
import "../CSS/App.css";

import {useEffect, useRef, useState} from "react";
import { useLocation } from "react-router-dom";

import { useCommand } from "../Hooks/useCommands.ts";
import { webClient } from "../api-clients.ts";

import type { Birdiesaysscore } from "../generated-ts-client";
import {StateleSSEClient} from "statele-sse";

const sse = new StateleSSEClient(
    "http://localhost:5000/api/WebApi/sse"
);

export default function BirdieSaysPage() {

    const location = useLocation();
    const name = location.state?.name || "Player";

    const { sendCommand } = useCommand();

    const [scores, setScores] = useState<Birdiesaysscore[]>([]);

    const cleanupRef = useRef<(() => void) | null>(null);

    // ---------------- START GAME ----------------

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

    // ---------------- REALTIME SCOREBOARD ----------------

    useEffect(() => {

        // cleanup previous listener
        if (cleanupRef.current) {
            cleanupRef.current();
        }

        const cleanup = sse.listen(

            // subscribe callback
            async (connectionId) => {

                console.log("SSE connection ID:", connectionId);

                return await webClient.getBirdieSaysScores(connectionId);
            },

            // data callback
            (data) => {

                console.log("Realtime update:", data);

                setScores(data);
            }
        );

        cleanupRef.current = cleanup;

        return () => cleanup?.();

    }, []);

    // ---------------- UI ----------------

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