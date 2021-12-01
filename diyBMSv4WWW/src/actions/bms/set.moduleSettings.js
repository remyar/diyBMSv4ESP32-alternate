import createAction from '../../middleware/actions';

async function setModuleSettings(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("savesetting.json",val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(setModuleSettings);