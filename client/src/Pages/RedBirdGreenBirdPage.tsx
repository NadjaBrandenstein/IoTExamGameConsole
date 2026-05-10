import "../CSS/RedBirdGreenBird.css";

export default function RedBirdGreenBirdPage() {
    return (
        <div className="redbird-page">
            <div className="redbird-rules">Rules</div>

            <div className="redbird-content">
                <h1>RED BIRD GREEN BIRD</h1>

                <div className="redbird-score-box">
                    <p>Player 1 - 00:10.000</p>
                </div>

                <button className="redbird-start-btn">
                    START GAME
                </button>
            </div>
        </div>
    );
}