class UserTakasusController < ApplicationController
  # GET /user_takasus
  # GET /user_takasus.json
  def index
    @user_takasus = UserTakasu.all

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @user_takasus }
    end
  end

  # GET /user_takasus/1
  # GET /user_takasus/1.json
  def show
    @user_takasu = UserTakasu.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @user_takasu }
    end
  end

  # GET /user_takasus/new
  # GET /user_takasus/new.json
  def new
    @user_takasu = UserTakasu.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @user_takasu }
    end
  end

  # GET /user_takasus/1/edit
  def edit
    @user_takasu = UserTakasu.find(params[:id])
  end

  # POST /user_takasus
  # POST /user_takasus.json
  def create
    point_new = params[:point]
    if User.all.count >= 10
      point = User.first.point
      user = User.first
      User.all.each do |u|
        if u.point < point
          user = u
          point = u.point
        end
      end
      if user.point < point_new.to_i
        user.destroy
        @user = User.new(name: params[:name], point: params[:point])
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
      @user = User.new(name: params[:name], point: params[:point])
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

  # PUT /user_takasus/1
  # PUT /user_takasus/1.json
  def update
    @user_takasu = UserTakasu.find(params[:id])

    respond_to do |format|
      if @user_takasu.update_attributes(params[:user_takasu])
        format.html { redirect_to @user_takasu, notice: 'User takasu was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @user_takasu.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /user_takasus/1
  # DELETE /user_takasus/1.json
  def destroy
    @user_takasu = UserTakasu.find(params[:id])
    @user_takasu.destroy

    respond_to do |format|
      format.html { redirect_to user_takasus_url }
      format.json { head :no_content }
    end
  end
end
