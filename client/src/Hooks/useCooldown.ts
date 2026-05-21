import {useEffect, useState} from "react";

export function useCooldown() {
    const [cooldown, setCooldown] = useState(0);

    useEffect(() => {
        if(cooldown <= 0) return;

        const timer = setInterval(() => {
            setCooldown((prev) => prev -1);
        }, 1000);

        return () => clearInterval(timer);
    }, [cooldown]);

    const startCooldown = (seconds: number) => {
        setCooldown(seconds);
    };

    return {
        cooldown,
        startCooldown
    };
}
