import createAction from '../../middleware/actions';

async function getBankSettings({ getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("getbankconfig.json");
        return obj;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getBankSettings);