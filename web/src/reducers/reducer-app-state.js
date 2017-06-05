export default function(state={panel: "d-pad"}, action) {
    switch (action.type) {
        case "CHANGE_PANEL":
            return {
                panel: action.payload
            };
    }
    return state;
}