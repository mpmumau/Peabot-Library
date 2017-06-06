import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';
import {testAction} from '../actions/actions';

import '../../scss/gamepad-component.scss'

class GamepadComponent extends Component {

    constructor(props) {
        super(props);
        this.state = {
            is_moving: false
        };
    }

    moveRobot(mvmt_type) {
        var mvmt_name;
        var reverse;

        switch (mvmt_type) {
            case "up":
                mvmt_name = "walk"
                reverse = false;
                break;
            case "down":
                reverse = true;
                mvmt_name = "walk"
                break;
            case "left":
                reverse = false;
                mvmt_name = "strafe"
                break;
            case "right":
                reverse = true;
                mvmt_name = "strafe"
                break;
        }
        fetch('http://ML_DEVNET_PIBOT:9976/event/' + mvmt_name, {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                cycles: 1,
                duration: 0.75,
                reverse: reverse
            })
        });
    }

    haltRobot() {
        fetch('http://ML_DEVNET_PIBOT:9976/event/halt', {
            method: 'POST'
        });
    }

    render() {
        return (
                <section className="gamepad-component">
                    <div className="d-pad">
                        <button className='up' onClick={() => this.moveRobot("up")}>
                            <span className="oi mega" data-glyph="arrow-thick-top"></span>
                        </button>

                        <button className='stop' onClick={() => this.haltRobot}>
                            <span className="oi mega" data-glyph="target"></span>
                        </button>

                        <button className='down' onClick={() => this.moveRobot("down")}>
                            <span className="oi mega" data-glyph="arrow-thick-bottom"></span>
                        </button>

                        <button className='right' onClick={() => this.moveRobot("right")}>
                            <span className="oi mega" data-glyph="arrow-thick-right"></span>
                        </button>

                        <button className='left' onClick={() => this.moveRobot("left")}>
                            <span className="oi mega" data-glyph="arrow-thick-left"></span>
                        </button>
                    </div>

                    <div className="buttons-panel">
                        <div className="distance">
                            <label>
                                Distance
                            </label>
                            <div className="data-val">
                                123.51cm
                            </div>
                        </div>

                        <div className="speed">
                            <label>
                                Speed
                            </label>
                            <input type="range" name="speed" />
                        </div>                           

                        <div className="rotate-buttons">
                            <button className="left">
                                <span className="oi mega" data-glyph="action-undo"></span>
                            </button>

                            <button className="right">
                                <span className="oi mega" data-glyph="action-redo"></span>
                            </button>                            
                        </div>
                    </div>
                </section>
            );
    }
}

function mapStateToProps(state) {
    if (!state.testReducer || !state.testReducer.first_obj) return {};
    return {
        first_obj: state.testReducer.first_obj
    }
}

function matchDispatchToProps(dispatch) {
    return bindActionCreators({
        testAction: testAction
    }, dispatch);
}

export default connect(mapStateToProps, matchDispatchToProps)(GamepadComponent);