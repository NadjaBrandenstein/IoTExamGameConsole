import { useEffect, useRef, useState } from "react";
import { StateleSSEClient } from "statele-sse";

import { finalBaseUrl } from "../BaseUrl.ts";

const sse = new StateleSSEClient(
    `${finalBaseUrl}/api/WebApi/sse`
);

export function useRealtimeScores<T>(
    fetchScores: (connectionId: string) => Promise<any>
) {
    const [scores, setScores] = useState<T[]>([]);

    const cleanupRef =
        useRef<(() => void) | null>(null);

    useEffect(() => {

        // cleanup old listener
        if (cleanupRef.current) {
            cleanupRef.current();
        }

        const cleanup = sse.listen(

            async (connectionId) => {

                console.log("SSE connection ID:", connectionId);

                return await fetchScores(connectionId);
            },

            (data: T[]) => {

                console.log("Realtime update:", data);

                setScores(data);
            }
        );

        cleanupRef.current = cleanup;

        return () => cleanup?.();

    }, []);

    return scores;
}