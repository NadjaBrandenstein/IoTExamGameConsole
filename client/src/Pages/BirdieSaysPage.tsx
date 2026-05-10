import "../CSS/BirdieSays.css";

export default function BirdieSaysPage() {
    return (
        <div className="simon-page">
            <div className="simon-rules">Rules</div>

            <div className="simon-content">
                <h1>BIRDIE SAYS</h1>

                <div className="simon-score-box">
                    <p>Player 1 - 520</p>
                </div>

                <button className="simon-start-btn">
                    START GAME
                </button>
            </div>
        </div>
    );
}