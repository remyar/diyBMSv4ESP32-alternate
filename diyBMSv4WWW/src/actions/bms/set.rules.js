import createAction from '../../middleware/actions';

async function setRules(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("saverules.json",val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(setRules);