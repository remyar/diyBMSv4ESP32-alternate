import createAction from '../../middleware/actions';

async function getAllBlocks({ getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("monitor2.json");
        return {
            
        };
    }
    catch(err){

    }
}