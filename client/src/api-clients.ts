import {
    CommandClient,
    WebApiClient
} from "./generated-ts-client.ts";

export const customFetch = async (
    url: RequestInfo,
    init?: RequestInit
) => {

    return await fetch(url, init);
};

export const apiUrl = import.meta.env.VITE_API_URL;

export const commandClient =
    new CommandClient(apiUrl, { fetch: customFetch });

export const webClient =
    new WebApiClient(apiUrl, { fetch: customFetch });