export default function(state=null, action) {
    switch (action.type) {
        case "TEST_ACTION":
            return {
                first_obj: action.payload
            };
    }
    return state;
}