import "../CSS/App.css";
import "../CSS/LandingPage.css";
import { useState } from "react";
import { useNavigate } from "react-router-dom";
import bird1 from "../assets/bird1.png";
import bird2 from "../assets/bird2.png";
import bird3 from "../assets/bird3.png";
import bird4 from "../assets/bird4.png";
import Bird from "../Components/Bird.tsx";

export default function LandingPage() {

    const [name, setName] = useState("");

    const navigate = useNavigate();

    const startGame = () => {

        if (!name.trim()) return;

        navigate("/menu", {
            state: { name }
        });
    };

    const canStart = name.trim().length > 0;

    return (
        <div className="page">

            {/* birds */}
            <Bird position="top-left" src={bird1} />
            <Bird position="top-right" src={bird2} />
            <Bird position="bottom-left" src={bird3} />
            <Bird position="bottom-right" src={bird4} />

            <div className="page-content">

                <h1 className="game-title">
                    RETRO BIRDS
                </h1>

                {/* START TITLE */}
                {canStart ? (

                    <h1
                        className="landing-title clickable"
                        onClick={startGame}
                    >
                        START GAME
                    </h1>

                ) : (

                    <h1 className="landing-title disabled-title">
                        START GAME
                    </h1>

                )}

                {/* INPUT */}
                <input
                    className="input"
                    placeholder="Insert Name"
                    value={name}
                    onChange={(e) => setName(e.target.value)}

                    onKeyDown={(e) => {

                        if (e.key === "Enter" && canStart) {
                            startGame();
                        }
                    }}
                />

            </div>

        </div>
    );
}