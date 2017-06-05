import React, {Component} from 'react';

class ActionsPanelSingleItemComponent extends Component {
    render() {
        return (
            <div className='single-action'>
                <button>
                    <span className="oi" data-glyph={this.props.iconType}></span>
                </button>

                <label>
                    {this.props.actionLabel}
                </label>
            </div>
        );
    }
}

export default ActionsPanelSingleItemComponent;