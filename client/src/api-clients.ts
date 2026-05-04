//import { TOKEN_KEY, tokenStorage } from "./Atoms/Auth.ts";
//import { AuthClient, CommandClient, WebClientClient} from "./generated-ts-client.ts";

export const customFetch = async (url: RequestInfo, init?: RequestInit) => {
    const token = tokenStorage.getItem(TOKEN_KEY, null);

    if (token) {
        init = {
            ...(init ?? {}),
            headers: {
                ...(init?.headers ?? {}),
                "Authorization": `Bearer ${token}`,
            },
        };
    }
    return await fetch(url, init);
};

export const apiUrl = import.meta.env.VITE_API_URL;

export const authClient = new AuthClient(apiUrl, { fetch: customFetch });
export const commandClient = new CommandClient(apiUrl, {fetch: customFetch});
export const webClient = new WebClientClient(apiUrl, { fetch: customFetch });
