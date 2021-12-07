import createAction from '../../middleware/actions';

async function setSdUnmount( { getState, extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("sdunmount.json", { unmount: 1 });
        return obj.success;
    }
    catch (err) {
        throw { message: err.message };
    }
}

export default createAction(setSdUnmount);