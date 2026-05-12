import {
    CommandClient,
    WebApiClient
} from "./generated-ts-client.ts";
import {finalBaseUrl} from "./BaseUrl.ts";


export const customFetch = async (
    url: RequestInfo,
    init?: RequestInit
) => {

    return await fetch(url, init);
};

// export const apiUrl = import.meta.env.VITE_API_URL;
export const apiUrl = finalBaseUrl;

if (!apiUrl) {
    throw new Error("API URL is missing");
}

export const commandClient =
    new CommandClient(apiUrl, { fetch: customFetch });

export const webClient =
    new WebApiClient(apiUrl, { fetch: customFetch });