import "../CSS/BirdieSays.css";
import "../CSS/App.css";
import BirdieSaysRules from "../Components/Rules/BirdieSaysRules.tsx";

//import {useEffect, useRef, useState} from "react";
import { useLocation, useNavigate } from "react-router-dom";

import { useCommand } from "../Hooks/useCommands.ts";
import { webClient } from "../api-clients.ts";

import type { Birdiesaysscore } from "../generated-ts-client";
// import {StateleSSEClient} from "statele-sse";

import bird1 from "../assets/bird1.png";
import bird2 from "../assets/bird2.png";
import bird3 from "../assets/bird3.png";
import bird4 from "../assets/bird4.png";
import Bird from "../Components/Bird.tsx";
import {useRealtimeScores} from "../Hooks/useRealtimeScores.ts";

// const sse = new StateleSSEClient(
//     "http://localhost:5000/api/WebApi/sse"
// );

export default function BirdieSaysPage() {

    const location = useLocation();
    const navigate = useNavigate();
    const name = location.state?.name || "Player";

    const { sendCommand } = useCommand();

    // const [scores, setScores] = useState<Birdiesaysscore[]>([]);
    //
    // const cleanupRef = useRef<(() => void) | null>(null);

    const scores = useRealtimeScores<Birdiesaysscore>(
        async (connectionId) => {

            const response =
                await webClient.getBirdieSaysScores(connectionId);

            return response || [];
        }
    );

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



    // useEffect(() => {
    //
    //     // cleanup previous listener
    //     if (cleanupRef.current) {
    //         cleanupRef.current();
    //     }
    //
    //     const cleanup = sse.listen(
    //
    //         // subscribe callback
    //         async (connectionId) => {
    //
    //             console.log("SSE connection ID:", connectionId);
    //
    //             return await webClient.getBirdieSaysScores(connectionId);
    //         },
    //
    //         // data callback
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
        <div className="birdie-page">

            {/* birds */}
            <Bird position="top-left" src={bird1} />
            <Bird position="top-right" src={bird2} />
            <Bird position="bottom-left" src={bird3} />
            <Bird position="bottom-right" src={bird4} />

            <div className="birdie-content">

                {/* HEADER */}
                <div className="birdie-header">

                    <h1>BIRDIE SAYS</h1>

                    <div className="rules-tooltip">

                        <span className="rules-icon">?</span>

                        <div className="tooltip-box">
                            <BirdieSaysRules />
                        </div>

                    </div>

                </div>

                {/* SCOREBOARD */}
                <div className="birdie-score-box">

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
                        className="birdie-start-btn"
                        onClick={() =>
                            navigate("/menu", {
                                state: { name }
                            })
                        }
                    >
                        RETURN TO MENU
                    </button>

                    <button
                        className="birdie-start-btn"
                        onClick={startGame}
                    >
                        START GAME
                    </button>
                </div>

            </div>
        </div>
    );
}