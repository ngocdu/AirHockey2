class AddRewardToUsers < ActiveRecord::Migration
  def change
    add_column :users, :reward, :integer
  end
end
