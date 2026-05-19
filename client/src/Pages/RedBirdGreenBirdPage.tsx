import "../CSS/RedBirdGreenBird.css";
import "../CSS/App.css";
import RedBirdGreenBirdRules from "../Components/Rules/RedBirdGreenBirdRules.tsx";

import bird1 from "../assets/bird1.png";
import bird2 from "../assets/bird2.png";
import bird3 from "../assets/bird3.png";
import bird4 from "../assets/bird4.png";

import Bird from "../Components/Bird.tsx";

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
            action: "start",
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

        const interval = setInterval(loadScores, 2000);

        return () => clearInterval(interval);

    }, []);

    return (
        <div className="redbird-page">

            {/* birds */}
            <Bird position="top-left" src={bird1} />
            <Bird position="top-right" src={bird2} />
            <Bird position="bottom-left" src={bird3} />
            <Bird position="bottom-right" src={bird4} />

            <div className="redbird-content">

                {/* HEADER */}
                <div className="redbird-header">

                    <h1>RED BIRD GREEN BIRD</h1>

                    <div className="rules-tooltip">

                        <span className="rules-icon">?</span>

                        <div className="tooltip-box">
                            <RedBirdGreenBirdRules />
                        </div>

                    </div>

                </div>

                {/* SCOREBOARD */}
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

                {/* BUTTON */}
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