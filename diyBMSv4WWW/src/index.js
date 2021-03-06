import React from 'react';
import ReactDOM from 'react-dom';

import "@fontsource/fira-code";

import { IntlProvider } from 'react-intl';
import { MemoryRouter } from "react-router-dom";
import NavigationProvider from './providers/navigation';
import StoreProvider from './providers/StoreProvider';
import SnackBarGenerator from './providers/snackBar';

import App from './App';
import api from './api';

// i18n datas
import localeData from './locales';

// WHITELIST
const persistConfig = {
    key: 'diyBMSv4',
    persist: true,
    whitelist: [
        "settings"
    ]
};


// Define user's language. Different browsers have the user locale defined
// on different fields on the `navigator` object, so we make sure to account
// for these different by checking all of them
const language = document.documentElement.lang != "%LANGUAGE%" ? document.documentElement.lang : ((navigator.languages && navigator.languages[0]) ||
    navigator.language ||
    navigator.userLanguage);

window.userLocale = language;

// Split locales with a region code
let languageWithoutRegionCode = language.toLowerCase().split(/[_-]+/)[0];

document.documentElement.lang = languageWithoutRegionCode;

window.userLocaleWithoutRegionCode = languageWithoutRegionCode;
localeData.setLocale(languageWithoutRegionCode);
// Try full locale, try locale without region code, fallback to 'en'
const messages = localeData[languageWithoutRegionCode] || localeData[language] || localeData.en;


ReactDOM.render(
    <React.StrictMode>
        <StoreProvider extra={{ api }} persistConfig={persistConfig} globalState={{ settings: { locale: "en" } }}>
            <MemoryRouter>
                <NavigationProvider>
                    <IntlProvider locale={language} messages={messages}>
                        <SnackBarGenerator>
                            <App />
                        </SnackBarGenerator>
                    </IntlProvider>
                </NavigationProvider>
            </MemoryRouter>
        </StoreProvider>
    </React.StrictMode>,
    document.getElementById('root')
);

