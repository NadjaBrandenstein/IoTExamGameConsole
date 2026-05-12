import "../CSS/RedBirdGreenBird.css";
import "../CSS/App.css";

import { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";

import { useCommand } from "../Hooks/useCommands.ts";
import { webClient } from "../api-clients.ts";

import type { Redbirdgreenbirdscore } from "../generated-ts-client";

export default function RedBirdGreenBirdPage() {

    const location = useLocation();

    const name = location.state?.name || "Player";

    const { sendCommand } = useCommand();

    const [scores, setScores] =
        useState<Redbirdgreenbirdscore[]>([]);

    const startGame = () => {

        if (!name) {
            return alert("Please enter a name!");
        }

        sendCommand(name, {
            action: "startGame",
            game: "redbirdgreenbird"
        });
    };

    useEffect(() => {

        const loadScores = async () => {

            try {

                const response =
                    await webClient.getRedbirdScores(undefined);

                setScores(response.data || []);

            } catch (err) {

                console.error("Failed to load scores", err);
            }
        };

        loadScores();

    }, []);

    return (
        <div className="redbird-page">

            <div className="redbird-rules">
                Rules
            </div>

            <div className="redbird-content">

                <h1>RED BIRD GREEN BIRD</h1>

                <div className="redbird-score-box">

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
                    className="redbird-start-btn"
                    onClick={startGame}
                >
                    START GAME
                </button>

            </div>
        </div>
    );
}