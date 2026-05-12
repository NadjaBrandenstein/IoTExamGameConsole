import "../CSS/WhackABird.css";
import "../CSS/App.css";

import { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";

import { useCommand } from "../Hooks/useCommands.ts";
import { webClient } from "../api-clients.ts";

import type { Whackamolescore } from "../generated-ts-client";

export default function WackAMolePage() {

    const location = useLocation();

    const name = location.state?.name || "Player";

    const { sendCommand } = useCommand();

    const [scores, setScores] = useState<Whackamolescore[]>([]);

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

    useEffect(() => {

        const loadScores = async () => {

            try {

                const response =
                    await webClient.getWhackamoleScores(undefined);

                setScores(response.data || []);

            } catch (err) {

                console.error("Failed to load scores", err);
            }
        };

        loadScores();

    }, []);

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