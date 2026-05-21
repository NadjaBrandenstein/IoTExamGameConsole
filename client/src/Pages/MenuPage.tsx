import "../CSS/App.css";
import "../CSS/MenuPage.css"
import { useState } from "react";
import { useLocation, useNavigate } from "react-router-dom";
import Bird from "../Components/Bird.tsx";
import WhackABirdRules from "../Components/Rules/WhackABirdRules.tsx";
import BirdieSaysRules from "../Components/Rules/BirdieSaysRules.tsx";
import RedBirdGreenBirdRules from "../Components/Rules/RedBirdGreenBirdRules.tsx";

import bird1 from "../assets/bird1.png";
import bird2 from "../assets/bird2.png";
import bird3 from "../assets/bird3.png";
import bird4 from "../assets/bird4.png";
import birdieSaysImg from "../assets/birdie_says.png";
import whackABirdImg from "../assets/whack a bird.png";
import redBirdGreenBirdImg from "../assets/Redbird_GreenBird.png";

const games = [
    {
        img: birdieSaysImg,
        rules: <BirdieSaysRules />,
        route: "/birdie-says"
    },
    {
        img: whackABirdImg,
        rules: <WhackABirdRules />,
        route: "/whack-a-bird"
    },
    {
        img: redBirdGreenBirdImg,
        rules: <RedBirdGreenBirdRules />,
        route: "/red-bird-green-bird"
    }
];

export default function MenuPage() {

    const navigate = useNavigate();
    const location = useLocation();
    const name = location.state?.name || "Player";

    const [index, setIndex] = useState(0);

    const next = () => setIndex((i) => (i + 1) % games.length);
    const prev = () => setIndex((i) => (i - 1 + games.length) % games.length);

    const current = games[index];
    const left = games[(index - 1 + games.length) % games.length];
    const right = games[(index + 1) % games.length];

    return (
        <div className="page">

            {/* birds */}
            <Bird position="top-left" src={bird1} />
            <Bird position="top-right" src={bird2} />
            <Bird position="bottom-left" src={bird3} />
            <Bird position="bottom-right" src={bird4} />

            <div className="page-content">

                <h1 className="title-menu">WELCOME {name}</h1>

                {/* CAROUSEL */}
                <div className="carousel">

                    {/* LEFT ARROW */}
                    <button className="menu-arrow" onClick={prev}>❮</button>

                    {/* TRACK (the 3 images) */}
                    <div className="carousel-track">

                        <div className="carousel-side left">
                            <img src={left.img} />
                        </div>

                        <div
                            className="carousel-center clickable-card"
                            onClick={() =>
                                navigate(current.route, {
                                    state: { name }
                                })
                            }
                        >
                            <img src={current.img} />
                        </div>

                        <div className="carousel-side right">
                            <img src={right.img} />
                        </div>

                    </div>

                    {/* RIGHT ARROW */}
                    <button className="menu-arrow" onClick={next}>❯</button>

                </div>
                
            </div>
            {/* RULES */}
            <div className="menu-rules-box">
                {current.rules}
            </div>
        </div>
    );
}