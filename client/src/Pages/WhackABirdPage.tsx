import "../CSS/WhackABird.css";

export default function WackAMolePage() {
    return (
        <div className="wack-page">
            <div className="wack-rules">Rules</div>

            <div className="wack-content">
                <h1>WACK-A-BIRD</h1>

                <div className="wack-score-box">
                    <p>Player 1 - 526</p>
                </div>

                <button className="wack-start-btn">
                    START GAME
                </button>
            </div>
        </div>
    );
}