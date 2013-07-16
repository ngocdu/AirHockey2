class CreateUserTakasus < ActiveRecord::Migration
  def change
    create_table :user_takasus do |t|
      t.string :name
      t.integer :point

      t.timestamps
    end
  end
end
