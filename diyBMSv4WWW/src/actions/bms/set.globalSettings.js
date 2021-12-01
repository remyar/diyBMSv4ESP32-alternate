import createAction from '../../middleware/actions';

async function setGlobalSettings(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("saveglobalsetting.json",val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(setGlobalSettings);