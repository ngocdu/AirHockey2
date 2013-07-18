class UsersController < ApplicationController
  # GET /users
  # GET /users.json
  def index
    @users = User.all
    respond_to do |format|
      format.html
      format.json { render json: @users }
    end
  end

  # GET /users/1
  # GET /users/1.json
  def show
    @user = User.find(params[:id])
    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @user }
    end
  end

  # GET /users/new
  # GET /users/new.json
  def new
    @user = User.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @user }
    end
  end

  # GET /users/1/edit
  def edit
    @user = User.find(params[:id])
  end

  # POST /users
  # POST /users.json
  def create
    user_first = User.where(name: params[:name], point: params[:point].to_i)
    if user_first.first != nil
      respond_to do |format|
        user_first.first.email = params[:email]
        if user_first.first.save
          if user_first.first.reward != 0
            UserMailer.welcome_email(user_first.first).deliver
            user_first.first.reward = 0
            user_first.first.save
          end
          format.html { redirect_to @user, notice: 'User was successfully updated.' }
          format.json { head :no_content }
        else
          format.html { render action: "edit" }
          format.json { render json: @user.errors, status: :unprocessable_entity }
        end
      end
    else
      point_new = params[:point]
    if User.all.count >= 10
      point = User.first.point
      point_max = User.first.point
      user = User.first
      user_max = User.first
      User.all.each do |u|
        if u.point < point
          user = u
          point = u.point
        elsif u.point > point_max
          user_max = u
          point_max = u.point
        end
      end

      if user.point < point_new.to_i
        user.destroy
        @user = nil
        if point_new.to_i > user_max.point
          @user = User.new(name: params[:name], point: params[:point],
            email: params[:email], reward: 1)
        else
          @user = User.new(name: params[:name], point: params[:point],
            email: params[:email], reward: params[:reward])
        end
        respond_to do |format|
          if @user.save
            format.html { redirect_to @users, notice: 'User was successfully created.' }
            format.json { render json: @users, status: :created, location: @user }
          else
            format.html { render action: "new" }
            format.json { render json: @user.errors, status: :unprocessable_entity }
          end
        end 
      end
    else
      @user = User.new(name: params[:name], point: params[:point], email: params[:email])
        respond_to do |format|
          if @user.save
            format.html { redirect_to @users, notice: 'User was successfully created.' }
            format.json { render json: @users, status: :created, location: @user }
          else
            format.html { render action: "new" }
            format.json { render json: @user.errors, status: :unprocessable_entity }
          end
        end
    end
    end 
  end
  
  def updateEmail
    binding.pry
  end
  # PUT /users/1
  # PUT /users/1.json
  def update
    @user = User.find(params[:id])
    respond_to do |format|
      if @user.update_attributes(params[:user])
        format.html { redirect_to @user, notice: 'User was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @user.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /users/1
  # DELETE /users/1.json
  def destroy
    @user = User.find(params[:id])
    @user.destroy

    respond_to do |format|
      format.html { redirect_to users_url }
      format.json { head :no_content }
    end
  end
end