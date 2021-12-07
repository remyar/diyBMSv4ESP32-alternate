/**
 * Load locales, send back object
 */
import { registerLocale, setDefaultLocale } from "react-datepicker";
import frDatepicker from 'date-fns/esm/locale/fr';
import enDatepicker from 'date-fns/esm/locale/en-US';
import esDatepicker from 'date-fns/esm/locale/es';
import itDatepicker from 'date-fns/esm/locale/it';

import de from './de.json';
import en from './en.json';
import es from './es.json';
import fr from './fr.json';
import hr from './hr.json';
import nl from './nl.json';
import pt from './pt.json';
import ru from './ru.json';

let actualLocale = 'en';

let DatePickerLang = [
    frDatepicker,
    enDatepicker,
];

const data = [
    { language: "German", code: "de" },
    { language: "English", code: "en" },
    { language: "Spanish", code: "es" },
    { language: "French", code: "fr" },
    { language: "Croatian", code: "hr" },
    { language: "Dutch", code: "nl" },
    { language: "Portuguese", code: "pt" },
    { language: "Russian", code: "ru" },
]

function setLanguage(language) {
    let d = data.find((el) => el.language == language);
    setLocale( d ? d.code : 'en' );
    return actualLocale;
}

function getLanguages() {
    return data;
}

function setLocale(code) {
    registerLocale(code, DatePickerLang.find((el) => el.code === code));
    actualLocale = code;
}

function getLocale(code = actualLocale) {
    return DatePickerLang.find((el) => el.code === code);
}

export default {
    setLocale,
    getLocale,
    setLanguage,
    getLanguages,
    de,
    en,
    es,
    fr,
    hr,
    nl,
    pt,
    ru
};