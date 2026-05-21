import "../CSS/WhackABird.css";
import "../CSS/App.css";
import bird1 from "../assets/bird1.png";
import bird2 from "../assets/bird2.png";
import bird3 from "../assets/bird3.png";
import bird4 from "../assets/bird4.png";
import Bird from "../Components/Bird.tsx";
import WhackABirdRules from "../Components/Rules/WhackABirdRules.tsx";

//import {useEffect, useRef, useState} from "react";
import { useLocation, useNavigate } from "react-router-dom";

import { useCommand } from "../Hooks/useCommands.ts";
import { webClient } from "../api-clients.ts";

import type { Whackabirdscore } from "../generated-ts-client";
//import {StateleSSEClient} from "statele-sse";
import {useRealtimeScores} from "../Hooks/useRealtimeScores.ts";
import {useCooldown} from "../Hooks/useCooldown.ts";

// const sse = new StateleSSEClient(
//     "http://localhost:5000/api/WebApi/sse"
// );

export default function WackAMolePage() {

    const location = useLocation();
    const navigate = useNavigate();
    const name = location.state?.name || "Player";

    const { sendCommand } = useCommand();
    // const [scores, setScores] = useState<Whackabirdscore[]>([]);
    //
    // const cleanupRef = useRef<(() => void) | null>(null);

    const {
        cooldown,
        startCooldown
    } = useCooldown();

    const scores = useRealtimeScores<Whackabirdscore>(
        async (connectionId) => {

            const response =
                await webClient.getWhackabirdScores(connectionId);

            return response || [];
        }
    );

    // ---------------- START GAME ----------------

    const startGame = () => {

        if(cooldown > 0) return;

        if (!name) {
            return alert("Please enter a name!");
        }

        sendCommand("firebeetle01", {
            game: "whackabird",
            action: "start",
            playerName: name
        });

        startCooldown(30);
    };

    // ---------------- REALTIME SSE ----------------

    // useEffect(() => {
    //
    //     if (cleanupRef.current) {
    //         cleanupRef.current();
    //     }
    //
    //     const cleanup = sse.listen(
    //
    //         async (connectionId) => {
    //
    //             console.log("SSE connection ID:", connectionId);
    //
    //             const response =
    //                 await webClient.getWhackabirdScores(connectionId);
    //
    //             return response;
    //         },
    //
    //         (data) => {
    //
    //             console.log("Realtime update:", data);
    //
    //             setScores(data);
    //         }
    //     );
    //
    //     cleanupRef.current = cleanup;
    //
    //     return () => cleanup?.();
    //
    // }, []);

    // ---------------- UI ----------------

    return (
        <div className="wack-page">

            {/* birds */}
            <Bird position="top-left" src={bird1} />
            <Bird position="top-right" src={bird2} />
            <Bird position="bottom-left" src={bird3} />
            <Bird position="bottom-right" src={bird4} />

            <div className="wack-content">

                {/* HEADER */}
                <div className="wack-header">

                    <h1>WHACK-A-BIRD</h1>

                    <div className="rules-tooltip">

                        <span className="rules-icon">?</span>

                        <div className="tooltip-box">
                            <WhackABirdRules />
                        </div>

                    </div>

                </div>

                {/* SCOREBOARD */}
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

                {/* BUTTON */}
                <div>
                    <button
                        className="wack-start-btn"
                        onClick={() =>
                            navigate("/menu", {
                                state: { name }
                            })
                        }
                    >
                        RETURN TO MENU
                    </button>

                    <button
                        className={`wack-start-btn ${cooldown > 0 ? "disabled" : ""}`}
                        onClick={startGame}
                        disabled={cooldown > 0}
                    >
                        <span>
                            START GAME
                        </span>

                        {cooldown > 0 && (
                            <span className="cooldown-overlay">
                                {cooldown}s
                            </span>
                        )}
                    </button>
                </div>

            </div>

        </div>
    );
}