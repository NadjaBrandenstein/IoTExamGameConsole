const isProduction = import.meta.env.PROD;

export const finalBaseUrl = isProduction
    ? import.meta.env.VITE_API_URL_PRODUCTION
    : import.meta.env.VITE_API_URL_DEVELOPMENT;