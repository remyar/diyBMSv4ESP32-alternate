import createAction from '../../middleware/actions';

async function getModule(val , { getState , extra }) {

    let api = extra.api;
    let state = getState();
    try {
        let obj = await api.get("modules.json?c=" + val);

        if ( state.modules == undefined){
            state.modules = [];
        }

        state.modules[val] = {
            ...obj.settings
        }

        return {
            modules : state.modules
        }
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getModule);