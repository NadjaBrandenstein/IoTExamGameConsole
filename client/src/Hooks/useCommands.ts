import type { GameCommandDto } from "../generated-ts-client";
import { commandClient } from "../api-clients.ts";
import { useState } from "react";

export const useCommand = () => {

    const [loading, setLoading] = useState(false);
    const [error, setError] = useState<string | null>(null);

    const sendCommand = async (deviceId: string, command: GameCommandDto) => {

        try {
            setLoading(true);
            setError(null);

            // const response = await commandClient.sendCommand(deviceId,command);
            //
            // return response;

            return await commandClient.sendCommand(deviceId, command);

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