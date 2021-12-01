import createAction from '../../middleware/actions';

async function identifyModule(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("identifyModule.json?c=" + val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(identifyModule);