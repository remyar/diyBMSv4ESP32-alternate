import createAction from '../../middleware/actions';

async function getSettings({ getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("settings.json");
        return {
            settings : obj.settings
        };
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getSettings);