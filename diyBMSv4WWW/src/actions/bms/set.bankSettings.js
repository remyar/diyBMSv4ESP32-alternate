import createAction from '../../middleware/actions';

async function setBankSettings(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("savebankconfig.json",val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(setBankSettings);