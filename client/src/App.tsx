import './CSS/App.css'
import {createBrowserRouter, RouterProvider} from "react-router-dom";
import LandingPage from "./Pages/LandingPage.tsx";
import MenuPage from "./Pages/MenuPage.tsx";
import WhackABirdPage from "./Pages/WhackABirdPage.tsx";
import BirdieSaysPage from "./Pages/BirdieSaysPage.tsx"
import RedBirdGreenBirdPage from "./Pages/RedBirdGreenBirdPage.tsx";

const App = () => {

    const router = createBrowserRouter([
        {
            path: "/",
            element: <LandingPage/>
        },
        {
            path: "/menu",
            element: <MenuPage/>
        },
        {
            path: "/whack-a-bird",
            element: <WhackABirdPage/>
        },
        {
            path: "/birdie-says",
            element: <BirdieSaysPage/>
        },
        {
            path: "/red-bird-green-bird",
            element: <RedBirdGreenBirdPage/>
        }
    ])
    return <RouterProvider router={router}/>
}

export default App
