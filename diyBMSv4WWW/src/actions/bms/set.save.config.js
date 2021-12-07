import createAction from '../../middleware/actions';

async function setSaveConfig( { getState, extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("saveconfigtofile.json", { save: 1 });
        return obj.success;
    }
    catch (err) {
        throw { message: err.message };
    }
}

export default createAction(setSaveConfig);