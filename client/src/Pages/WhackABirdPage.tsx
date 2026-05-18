import "../CSS/WhackABird.css";
import "../CSS/App.css";

import {useEffect, useRef, useState} from "react";
import { useLocation } from "react-router-dom";

import { useCommand } from "../Hooks/useCommands.ts";
import { webClient } from "../api-clients.ts";

import type { Whackabirdscore } from "../generated-ts-client";
import {StateleSSEClient} from "statele-sse";

const sse = new StateleSSEClient(
    "http://localhost:5000/api/WebApi/sse"
);

export default function WackAMolePage() {

    const location = useLocation();
    const name = location.state?.name || "Player";

    const { sendCommand } = useCommand();
    const [scores, setScores] = useState<Whackabirdscore[]>([]);

    const cleanupRef = useRef<(() => void) | null>(null);

    // ---------------- START GAME ----------------

    const startGame = () => {

        if (!name) {
            return alert("Please enter a name!");
        }

        sendCommand("firebeetle01", {
            game: "whackabird",
            action: "start",
            playerName: name
        });
    };

    // ---------------- REALTIME SSE ----------------

    useEffect(() => {

        if (cleanupRef.current) {
            cleanupRef.current();
        }

        const cleanup = sse.listen(

            async (connectionId) => {

                console.log("SSE connection ID:", connectionId);

                const response =
                    await webClient.getWhackabirdScores(connectionId);

                return response;
            },

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
        <div className="wack-page">

            <div className="wack-rules">
                Rules
            </div>

            <div className="wack-content">

                <h1>WACK-A-BIRD</h1>

                <div className="wack-score-box">

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
                    className="wack-start-btn"
                    onClick={startGame}
                >
                    START GAME
                </button>

            </div>
        </div>
    );
}