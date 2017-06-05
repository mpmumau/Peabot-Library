import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';
import {selectUser} from '../actions/actions';

class UserList extends Component {
    createListItems() {
        return this.props.users.map((user) => {
            return (
                <li 
                    onClick={() => this.props.selectUser(user)} 
                    key={user.id}
                >
                    {user.username}
                </li>
            );
        });
    }

    render() {
        return (
            <ul>
                {this.createListItems()}
            </ul>
        );
    }
}

function mapStateToProps(state) {
    return {
        users: state.users
    }
}

function matchDispatchToProps(dispatch) {
    return bindActionCreators({selectUser: selectUser}, dispatch);
}

export default connect(mapStateToProps, matchDispatchToProps)(UserList);