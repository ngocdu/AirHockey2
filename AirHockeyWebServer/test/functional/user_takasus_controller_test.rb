require 'test_helper'

class UserTakasusControllerTest < ActionController::TestCase
  setup do
    @user_takasu = user_takasus(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:user_takasus)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create user_takasu" do
    assert_difference('UserTakasu.count') do
      post :create, user_takasu: { name: @user_takasu.name, point: @user_takasu.point }
    end

    assert_redirected_to user_takasu_path(assigns(:user_takasu))
  end

  test "should show user_takasu" do
    get :show, id: @user_takasu
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @user_takasu
    assert_response :success
  end

  test "should update user_takasu" do
    put :update, id: @user_takasu, user_takasu: { name: @user_takasu.name, point: @user_takasu.point }
    assert_redirected_to user_takasu_path(assigns(:user_takasu))
  end

  test "should destroy user_takasu" do
    assert_difference('UserTakasu.count', -1) do
      delete :destroy, id: @user_takasu
    end

    assert_redirected_to user_takasus_path
  end
end
