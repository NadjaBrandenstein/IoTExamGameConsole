import "../CSS/App.css";
import { useState } from "react";
import { useNavigate } from "react-router-dom";

export default function LandingPage() {

    const [name, setName] = useState("");
    const navigate = useNavigate();

    const startGame = () => {
        if (!name.trim()) return;
        navigate("/menu", { state: { name } });
    };

    const canStart = name.trim().length > 0;

    return (
        <div className="page">

            {/* birds */}
            <div className="bird top-left">🐦</div>
            <div className="bird top-right">🐦</div>
            <div className="bird bottom-left">🕊</div>
            <div className="bird bottom-right">🐤</div>

            <div className="page-content">

                {/* H1 becomes interactive */}
                {canStart ? (
                    <h1
                        className="title clickable"
                        onClick={startGame}
                    >
                        START GAME
                    </h1>
                ) : (
                    <h1 className="title disabled-title">
                        START GAME
                    </h1>
                )}

                <input
                    className="input"
                    placeholder="Insert Name"
                    value={name}
                    onChange={(e) => setName(e.target.value)}
                />

            </div>
        </div>
    );
}