class UserMailer < ActionMailer::Base
  default from: "ngocduk54a2@gmail.com"
  def welcome_email user
    @user = user
    # id = @user.id
    # verify = @user.verify
    # host = "http://localhost:3000/users"
    # @url = host  + "&"  + verify
    @code = Digest::SHA1.hexdigest(user.name)
    @name = user.name
    mail(to: user.email, subject: "Reward from Airhockey game")
  end
end