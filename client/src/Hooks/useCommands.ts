import type { GameCommandDto } from "../generated-ts-client";
import { commandClient } from "../api-clients.ts";
import { useState } from "react";

export const useCommand = () => {

    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    const sendCommand = async (command: GameCommandDto) => {

        try {
            setLoading(true);
            setError(null);

            const response = await commandClient.sendCommand(command);

            return response;

        } catch (err) {

            console.error(err);

            if (err instanceof Error) {
                setError(err.message);
            } else {
                setError("Failed to send command");
            }

        } finally {
            setLoading(false);
        }
    };

    return {
        loading,
        error,
        sendCommand
    };
};