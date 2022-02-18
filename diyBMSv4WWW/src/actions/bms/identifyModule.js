import createAction from '../../middleware/actions';

async function identifyModule(ctrl , idx , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("identifyModule.json?c=" + ctrl + "&cmi=" + idx);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(identifyModule);