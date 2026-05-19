import { useNavigate } from "react-router-dom";

type Props = {
    position: string;
    src: string;
};

export default function Bird({ position, src }: Props) {

    const navigate = useNavigate();

    return (
        <img
            className={`bird ${position}`}
            src={src}
            onClick={() => navigate(-1)}
        />
    );
}