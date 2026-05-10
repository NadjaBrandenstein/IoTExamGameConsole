type BirdProps = {
    position: string;
    src: string;
};

export default function Bird({ position, src }: BirdProps) {
    return (
        <img
            src={src}
            className={`bird ${position}`}
            alt="bird"
        />
    );
}